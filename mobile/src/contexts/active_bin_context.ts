import { createContext } from "react";
import { ActiveBinContextType } from "../types/contexts";

const ActiveBinContext = createContext<ActiveBinContextType | undefined>(
  undefined,
);

export default ActiveBinContext;
