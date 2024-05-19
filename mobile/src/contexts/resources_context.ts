import { createContext } from "react";
import { ResourcesContextType } from "../types/contexts";

const ResourcesContext = createContext<ResourcesContextType | undefined>(
  undefined,
);

export default ResourcesContext;
