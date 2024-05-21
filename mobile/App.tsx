import "react-native-gesture-handler";
import { useEffect, useState } from "react";
import { Resources, fetchResources } from "./src/api/resources";
import { ActiveBinContext, ResourcesContext } from "./src/contexts";
import Drawer from "./src/navigation/drawer/Drawer";

export default function App() {
  const [resources, setResources] = useState<Resources>();
  const [activeBin, setActiveBin] = useState<string | undefined>();

  const fetchData = async () => {
    try {
      const data = await fetchResources();
      setResources(data);

      return data;
    } catch (error) {
      console.error(error);
    }
  };

  useEffect(() => {
    fetchData()
      .then((resources) => setActiveBin(resources?.bins[0].id))
      .catch(console.error);
  }, []);

  return (
    <ResourcesContext.Provider value={{ data: resources, update: fetchData }}>
      <ActiveBinContext.Provider
        value={{ data: activeBin, update: setActiveBin }}
      >
        <Drawer />
      </ActiveBinContext.Provider>
    </ResourcesContext.Provider>
  );
}
