import Resources from "./models/resources";

const URL = "http://192.168.100.208:5000/";

export default async function fetchResources(): Promise<Resources> {
  const response = await fetch(URL);
  const resources = await response.json() as Resources;

  return resources;
}
