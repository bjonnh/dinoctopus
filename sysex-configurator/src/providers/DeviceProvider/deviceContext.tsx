import {createContext} from "react";
import {DeviceContextType} from "./DeviceContextType.ts";

export const DeviceContext = createContext<DeviceContextType | undefined>(undefined)
