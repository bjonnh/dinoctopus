import {DeviceProvider} from "../../providers/DeviceProvider"
import {useEffect, useState} from "react"
import DeviceAccordion from "../DeviceList"
import Matrix from "../Matrix"
import {Button, Checkbox, Chip, FormControlLabel, FormGroup, Tooltip} from "@mui/material"
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
                        <Tooltip title="Get the values from the device (this should not really be needed)" arrow>
                            <Button onClick={() => {
                                dump(undefined)
                            }} disabled={busyState}>Dump</Button>
                        </Tooltip>
                        <Tooltip title="Send the values to the device (useful only when Direct update is off)" arrow>
                            <Button onClick={send} disabled={busyState || immediateUpdate}>Send</Button>
                        </Tooltip>
                        <Tooltip title="Load the values stored in the device" arrow>
                            <Button onClick={load} disabled={busyState}>Load</Button>
                        </Tooltip>
                        <Tooltip title="Save the values to the device, this will be the default when it boots" arrow>
                            <Button onClick={save} disabled={busyState}>Save</Button>
                        </Tooltip>
                        <Tooltip title="Send the updates directly to the device" arrow>
                        <FormControlLabel control={<Checkbox checked={immediateUpdate}
                                                             onChange={(e) => setImmediateUpdate(e.target.checked)}></Checkbox>}
                                          label="Immediate update"></FormControlLabel>
                        </Tooltip>
                        {lastStatus && (<Chip label={`Last status: ${lastStatus}`}></Chip>)}
                    </FormGroup>
                    <hr/>
                </div>)}

            </div>
        </DeviceProvider>
    </>)
}

export default Main
