import {useEffect, useRef, useState} from 'react'
import {Input, Output, WebMidi} from "webmidi";

import './App.css'
import Matrix from "./components/Matrix";
import {Accordion, Button} from "@mui/material";
import DeviceAccordion from "./components/DeviceList";

interface IOData {
    input: Input | undefined
    output: Output | undefined
}

const DEVICE_NAME = "DINoctopus:DINoctopus MIDI 1"
const MATRIX_SIZE = 25

function App() {
    const [midiInputs, setmidiInputs] = useState<Input[]>([])
    const [midiOutputs, setmidiOutputs] = useState<Output[]>([])
    const [selectedDevice, setSelectedDevice] = useState<IOData>({input: undefined, output: undefined})
    const [showDevices, setShowDevices] = useState(false)
    const [deviceState, setDeviceState] = useState<DeviceState>({size: 25, matrix: Array(MATRIX_SIZE).fill(0)})
    const [loadingState, setLoadingState] = useState(false)
    const [lastStatus, setLastStatus] = useState<string | undefined>(undefined)
    const [requireDump, setRequireDump] = useState(false)
    const requireDumpRef = useRef(requireDump)

    useEffect(() => {
        requireDumpRef.current = requireDump; // Update the ref when state changes
    }, [requireDump]);

    useEffect(() => {
        WebMidi.enable({sysex: true})
            .then(() => {
                setmidiInputs(WebMidi.inputs)
                setmidiOutputs(WebMidi.outputs)
                const matching = WebMidi.inputs.find(obj => obj.name.includes(DEVICE_NAME))
                if (matching) handleDeviceClick(matching.name)
            })
            .catch(err => console.log(err));
    }, []);

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
                    setLoadingState(false)
                    const matrix = Array(MATRIX_SIZE).fill(0)
                    for (let i = 1; i < msg.length - 1; i += 4) {
                        matrix[(i - 1) / 4] = msg[i] + (msg[i + 1] << 8) + (msg[i + 2] << 16) + (msg[i + 3] << 24);
                    }
                    setDeviceState({size: MATRIX_SIZE, matrix: matrix})
                } else if (msg.length == 3) {
                    if (requireDumpRef.current) {
                        setTimeout(() => {
                            dump_device()
                        }) // We need that or we can't get the results back
                        setRequireDump(false)
                    }
                    setLoadingState(false)
                    if (msg[1] == 0x60) {
                        setLastStatus("ACK")
                    } else if (msg[0] == 0x61) {
                        setLastStatus("NACK")
                    } else {
                        setLastStatus("Unknown status: 0x" + msg[1].toString(16))
                    }
                }
            }
        })


        setTimeout(() => {
            dump_device({ input: newInput, output: newOutput })
        }, 100)
    }

    function onCheckboxChange(index: number, newValue: number) {
        const oldDeviceState = deviceState
        oldDeviceState.matrix[index] = newValue
        setDeviceState({size: MATRIX_SIZE, matrix: oldDeviceState.matrix})
    }

    function dump_device(ioData: IOData|undefined) {
        console.log("Dumping")
        const device = ioData || selectedDevice
        if (device.output === undefined) return;
        setLoadingState(true)
        device.output.send(new Uint8Array([0xf0, 0x44, 0x49, 0x4E, 0x4F, 0x01, 0xF7]))
    }

    function numbersToList(numbers: number[]): number[] {
        const list: number[] = []

        for (const num of numbers) {
            const byte1 = num & 0x7F
            const byte2 = (num >> 8) & 0x7F
            const byte3 = (num >> 16) & 0x7F
            const byte4 = (num >> 24) & 0x7F

            list.push(byte1, byte2, byte3, byte4)
        }

        return list
    }

    function send_device() {
        if (selectedDevice.output === undefined) return;
        setLoadingState(true)
        setLastStatus(undefined)
        selectedDevice.output.send(new Uint8Array([0xf0, 0x44, 0x49, 0x4E, 0x4F, 0x02].concat(numbersToList(deviceState.matrix).concat([0xF7]))))
    }

    function load_device() {
        if (selectedDevice.output === undefined) return;
        setLoadingState(true)
        setLastStatus(undefined)
        setRequireDump(true)
        selectedDevice.output.send(new Uint8Array([0xf0, 0x44, 0x49, 0x4E, 0x4F, 0x06, 0xF7]))
    }

    function save_device() {
        if (selectedDevice.output === undefined) return;
        setLoadingState(true)
        setLastStatus(undefined)
        setRequireDump(true)
        selectedDevice.output.send(new Uint8Array([0xf0, 0x44, 0x49, 0x4E, 0x4F, 0x05, 0xF7]))
    }

    return (<>
        <h1>DINoctopus</h1>
        <div className="card">
            <DeviceAccordion midiInputs={midiInputs} midiOutputs={midiOutputs} handleDeviceClick={handleDeviceClick} selectedDevice={selectedDevice}></DeviceAccordion>

            {selectedDevice.input && selectedDevice.output && (<div>

                <Matrix deviceState={deviceState} onCheckboxChange={onCheckboxChange} disabled={loadingState}></Matrix>

                <Button onClick={dump_device} disabled={loadingState}>Dump</Button>
                <Button onClick={send_device} disabled={loadingState}>Send</Button>
                <Button onClick={load_device} disabled={loadingState}>Load</Button>
                <Button onClick={save_device} disabled={loadingState}>Save</Button>
                <hr/>
                Last status: {lastStatus}
            </div>)}

        </div>
    </>)


}

export default App
