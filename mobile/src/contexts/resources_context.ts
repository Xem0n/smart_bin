import { createContext } from "react"
import { Resources } from "../api/resources";

const ResourcesContext = createContext<Resources | undefined>(undefined);

export default ResourcesContext;
