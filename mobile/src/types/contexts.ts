import { Resources } from "../api/resources";

export interface ResourcesContextType {
  data: Resources | undefined;
  update: () => void;
}

export interface ActiveBinContextType {
  data: string | undefined;
  update: (newBin: string) => void;
}
