import Resources from "./models/resources";
import data from "./mock/resources.json";

// todo: change to proper url
// const URL = "./mock/resources.json";

export default async function fetchResources(): Promise<Resources> {
  // const response = await fetch(URL);
  // const resources = await response.json() as Resources;

  const resources = data as Resources;

  return resources;
}
