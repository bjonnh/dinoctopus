import {useEffect, useRef, useState} from "react"
import {Input, Output, WebMidi} from "webmidi"
import {DeviceState} from "../../interfaces/DeviceState.tsx"
import {IOData} from "./IOData.tsx"
import {DeviceContext} from "./deviceContext.tsx"


const DEVICE_NAME = "DINoctopus:DINoctopus MIDI 1"
const MATRIX_SIZE = 25
const DEVICE_SIGNATURE = [0x44, 0x49, 0x4E, 0x4F]

const initialState = {
    connected: false, size: MATRIX_SIZE, matrix: Array(MATRIX_SIZE).fill(0)
}

export function DeviceProvider({children}: { children: React.ReactNode }) {
    const [midiInputs, setMidiInputs] = useState<Input[]>([])
    const [midiOutputs, setMidiOutputs] = useState<Output[]>([])
    const [deviceState, setDeviceState] = useState<DeviceState>(initialState)
    const [selectedDevice, setSelectedDevice] = useState<IOData>({input: undefined, output: undefined})
    const [busyState, setBusyState] = useState(false)
    const [lastStatus, setLastStatus] = useState<string | undefined>(undefined)
    const [requireDump, setRequireDump] = useState(false)
    const requireDumpRef = useRef(requireDump)

    useEffect(() => {
        requireDumpRef.current = requireDump; // Update the ref when state changes
    }, [requireDump]);

    const connectDevice = () => {
        WebMidi.enable({sysex: true})
            .then(() => {
                setMidiInputs(WebMidi.inputs)
                setMidiOutputs(WebMidi.outputs)
                const matching = WebMidi.inputs.find(obj => obj.name.includes(DEVICE_NAME))
                if (matching) {
                    handleDeviceClick(matching.name)
                }
            })
            .catch(err => console.log(err));
        // eslint-disable-next-line react-hooks/exhaustive-deps
        // Implement device connection logic here
        setDeviceState({...deviceState, connected: true});
    };

    function handleDeviceClick(name: string) {
        selectedDevice.input?.removeListener("sysex")
        const newInput = WebMidi.inputs.find(obj => obj.name === name);
        const newOutput = WebMidi.outputs.find(obj => obj.name === name)

        setSelectedDevice({
            input: newInput, output: newOutput
        })
        if (newInput === undefined || newOutput === undefined) return
        newInput.removeListener("sysex")
        newInput.addListener("sysex", (data) => {
            if (data.message?.data) {
                const msg = data.message.data
                if (msg.length == 102) {
                    const matrix = Array(MATRIX_SIZE).fill(0)
                    for (let i = 1; i < msg.length - 1; i += 4) {
                        matrix[(i - 1) / 4] = msg[i] + (msg[i + 1] << 8) + (msg[i + 2] << 16) + (msg[i + 3] << 24);
                    }
                    setDeviceState({...deviceState, matrix: matrix})
                    setBusyState(false)
                } else if (msg.length == 3) {
                    if (requireDumpRef.current) {
                        setTimeout(() => {
                            dump()
                        }) // We need that or we can't get the results back
                        setRequireDump(false)
                    }
                    if (msg[1] == 0x60) {
                        setLastStatus("ACK")
                    } else if (msg[0] == 0x61) {
                        setLastStatus("NACK")
                    } else {
                        setLastStatus("Unknown status: 0x" + msg[1].toString(16))
                    }
                    setBusyState(false)
                }
            }
        })

        setTimeout(() => {
            dump({input: newInput, output: newOutput})
        }, 100)
    }

    function numberToInt32_7b(numbers: number[]): number[] {
        const list: number[] = []

        for (const num of numbers) {
            list.push(num & 0x7F, (num >> 8) & 0x7F, (num >> 16) & 0x7F, (num >> 24) & 0x7F)
        }

        return list
    }

    function updateRoutingElement(index: number, newValue: number) {
        const oldDeviceState = deviceState
        oldDeviceState.matrix[index] = newValue
        setDeviceState({...deviceState, matrix: oldDeviceState.matrix})
    }

    const disconnectDevice = () => {
        setDeviceState({...deviceState, connected: false});
    }

    function markBusy() {
        setBusyState(true)
        setLastStatus(undefined)
    }

    function sendSysEx(command: number, data: number[] = [], ioData: IOData | undefined = undefined) {
        const device = ioData || selectedDevice
        if (device.output === undefined) return;
        markBusy()
        device.output.send([0xf0].concat(DEVICE_SIGNATURE.concat([command].concat(data.concat([0xf7])))))
    }

    function send() {
        if (selectedDevice.output === undefined) return;
        sendSysEx(0x02, numberToInt32_7b(deviceState.matrix))
    }

    function load() {
        if (selectedDevice.output === undefined) return;
        setRequireDump(true)
        sendSysEx(0x06)
    }

    function save() {
        if (selectedDevice.output === undefined) return;
        setRequireDump(true)
        sendSysEx(0x05)
    }

    function dump(ioData: IOData | undefined = undefined) {
        sendSysEx(0x01, [], ioData)
    }

    return (<DeviceContext.Provider value={{
            connectDevice,
            disconnectDevice,
            handleDeviceClick,
            updateRoutingElement,
            selectedDevice,
            send,
            load,
            save,
            dump,
            deviceState,
            busyState,
            lastStatus,
            midiInputs,
            midiOutputs
        }}>
            {children}
        </DeviceContext.Provider>)
}


