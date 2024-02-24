import {DeviceProvider} from "../../providers/DeviceProvider"
import {useEffect, useState} from "react"
import DeviceAccordion from "../DeviceList"
import Matrix from "../Matrix"
import {Button, Checkbox, Chip, FormControlLabel, FormGroup} from "@mui/material"
import {useDevice} from "../../providers/DeviceProvider/useDevice.tsx"

function Main() {
    const [immediateUpdate, setImmediateUpdate] = useState(true)

    const {
        connectDevice,
        handleDeviceClick,
        updateRoutingElement,
        send,
        load,
        save,
        dump,
        selectedDevice,
        deviceState,
        busyState,
        lastStatus,
        midiInputs,
        midiOutputs
    } = useDevice();

    useEffect(() => {
        connectDevice()
    }, [])

    function onMatrixUpdate(index: number, newValue: number) {
        updateRoutingElement(index, newValue)
        if (immediateUpdate) {
            send()
        }
    }

    return (<>
        <DeviceProvider>
            <div className="card">
                <DeviceAccordion midiInputs={midiInputs} midiOutputs={midiOutputs} handleDeviceClick={handleDeviceClick}
                                 selectedDevice={selectedDevice}></DeviceAccordion>

                {selectedDevice.input && selectedDevice.output && (<div>
                    <Matrix deviceState={deviceState} onUpdate={onMatrixUpdate}
                            disabled={busyState}></Matrix>

                    <FormGroup row>
                        <Button onClick={() => {
                            dump(undefined)
                        }} disabled={busyState}>Dump</Button>
                        <Button onClick={send} disabled={busyState}>Send</Button>
                        <Button onClick={load} disabled={busyState}>Load</Button>
                        <Button onClick={save} disabled={busyState}>Save</Button>
                        <FormControlLabel control={<Checkbox checked={immediateUpdate}
                                                             onChange={(e) => setImmediateUpdate(e.target.checked)}></Checkbox>}
                                          label="Direct update"></FormControlLabel>
                        {lastStatus && (<Chip label={`Last status: ${lastStatus}`}></Chip>)}
                    </FormGroup>
                    <hr/>
                </div>)}

            </div>
        </DeviceProvider>
    </>)
}

export default Main
