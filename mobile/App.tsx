import "react-native-gesture-handler";
import { useEffect, useState } from "react";
import { Resources, fetchResources } from "./src/api/resources";
import { ActiveBinContext, ResourcesContext } from "./src/contexts";
import Drawer from "./src/navigation/drawer/Drawer";

export default function App() {
  const [resources, setResources] = useState<Resources>();
  const [activeBin, setActiveBin] = useState<string | undefined>();

  const fetch = async () => {
    try {
      const data = await fetchResources();
      setResources(data);
    } catch (error) {
      console.error(error);
    }
  };

  useEffect(() => {
    fetch()
      .then(() => setActiveBin(resources?.bins[0].id))
      .catch(console.error);
  });

  return (
    <ResourcesContext.Provider value={{ data: resources, update: fetch }}>
      <ActiveBinContext.Provider
        value={{ data: activeBin, update: setActiveBin }}
      >
        <Drawer />
      </ActiveBinContext.Provider>
    </ResourcesContext.Provider>
  );
}
