import "react-native-gesture-handler";
import { useEffect, useState } from "react";
import { Resources, fetchResources } from "./src/api/resources";
import { ResourcesContext } from "./src/contexts";
import Drawer from "./src/navigation/drawer/Drawer";

export default function App() {
  const [resources, setResources] = useState<Resources>();

  const fetch = async () => {
    try {
      const data = await fetchResources();
      setResources(data);
    } catch (error) {
      console.error(error);
    }
  };

  useEffect(() => {
    fetch();
  });

  return (
    <ResourcesContext.Provider value={{ data: resources, update: fetch }}>
      <Drawer />
    </ResourcesContext.Provider>
  );
}
