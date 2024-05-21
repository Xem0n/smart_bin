import Resources from "./models/resources";
import { URL } from "../../constants/api";

export default async function fetchResources(): Promise<Resources> {
  const response = await fetch(`${URL}/`);
  const resources = await response.json() as Resources;

  return resources;
}
