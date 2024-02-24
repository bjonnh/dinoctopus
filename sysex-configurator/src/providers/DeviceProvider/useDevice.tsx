import {useContext} from "react";
import {DeviceContext} from "./deviceContext.tsx";


export function useDevice() {
    const context = useContext(DeviceContext);
    if (!context) {
        throw new Error('useDevice must be used within a DeviceProvider');
    }
    return context;
}
