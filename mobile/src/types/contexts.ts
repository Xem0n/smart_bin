import { Resources } from "../api/resources";

export interface ResourcesContextType {
  data: Resources | undefined;
  update: () => void;
}
