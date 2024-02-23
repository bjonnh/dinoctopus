import {useEffect, useState} from 'react'
import {Input, Output, WebMidi} from "webmidi";

import './App.css'

interface IOData {
    input: Input | undefined
    output: Output | undefined
}

function App() {
    const [midiInputs, setmidiInputs] = useState<Input[]>([])
    const [midiOutputs, setmidiOutputs] = useState<Output[]>([])
    const [selectedDevice, setSelectedDevice] = useState<IOData>({input: undefined, output: undefined})
    const [lastMessage, setLastMessage] = useState<number[] | undefined>(undefined)

    useEffect(() => {
        WebMidi.enable({sysex: true})
            .then(() => {
                setmidiInputs(WebMidi.inputs)
                setmidiOutputs(WebMidi.outputs)
            })
            .catch(err => console.log(err));
    }, []);

    function handleDeviceClick(name: string) {
        selectedDevice.input?.removeListener("sysex")
        const newInput = midiInputs.find(obj => obj.name === name);
        const newOutput = midiOutputs.find(obj => obj.name === name)

        setSelectedDevice({
            input: newInput, output: newOutput
        })
        if (newInput === undefined || newOutput === undefined) return
        console.log(name)
        console.log(newInput.name)
        console.log(newOutput.name)
        newInput.addListener("sysex", (data) => {
            setLastMessage(data.message?.data)
            console.log(data)
        })

    }

    return (<>
        <h1>DINoctopus</h1>
        <div className="card">
            <div style={{display: "inline-block"}}>
                <h2>Midi inputs</h2>
                <ul>
                    {midiInputs.map((input, index) => (<li key={index} onClick={() => handleDeviceClick(input.name)}
                                                           style={{
                                                               cursor: 'pointer',
                                                               fontWeight: selectedDevice.input && selectedDevice.input.name === input.name ? 'bold' : 'normal'
                                                           }}
                    >
                        {input.name}
                    </li>))}
                </ul>
            </div>
            <div style={{display: "inline-block"}}>
                <h2>Midi outputs</h2>
                <ul>
                    {midiOutputs.map((output, index) => (<li key={index} onClick={() => handleDeviceClick(output.name)}
                                                             style={{
                                                                 cursor: 'pointer',
                                                                 fontWeight: selectedDevice.output && selectedDevice.output.name === output.name ? 'bold' : 'normal'
                                                             }}
                    >
                        {output.name}
                    </li>))}
                </ul>
            </div>
            {selectedDevice.input && selectedDevice.output && (<div>
                <button onClick={() => {
                    if (selectedDevice.output === undefined) return;
                    selectedDevice.output.send(new Uint8Array([0xf0, 0x44, 0x49, 0x4E, 0x4F, 0x01, 0xF7]))
                }}>
                    Dump
                </button>
            </div>)}

            {lastMessage && (<div>{lastMessage.map((byteValue) => (
                <span style={{margin: "1em", display: "inline-block",}}>{byteValue}</span>))}</div>)}

        </div>
    </>)
}

export default App
