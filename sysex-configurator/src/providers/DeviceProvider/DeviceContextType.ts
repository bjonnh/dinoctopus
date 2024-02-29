import {DeviceState} from "../../interfaces/DeviceState.tsx";
import {Input, Output} from "webmidi";
import {IOData} from "./IOData.tsx";

export interface DeviceContextType {
    connectDevice: () => void
    disconnectDevice: () => void
    handleDeviceClick: (name: string) => void
    updateRoutingElement: (index: number, newValue: number) => void
    load: () => void
    save: () => void
    dump: (iodata: IOData | undefined) => void
    send: () => void
    selectedDevice: IOData
    deviceState: DeviceState
    busyState: boolean
    lastStatus: string | undefined
    midiInputs: Input[]
    midiOutputs: Output[]
}
