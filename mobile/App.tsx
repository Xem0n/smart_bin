import "react-native-gesture-handler";
import { useEffect, useState } from "react";
import { Resources, fetchResources } from "./src/api/resources";
import { ResourcesContext } from "./src/contexts";
import Drawer from "./src/navigation/drawer/Drawer";

export default function App() {
  const [resources, setResources] = useState<Resources>();

  useEffect(() => {
    fetchResources()
      .then((data) => setResources(data))
      .catch(() => {});
  });

  return (
    <ResourcesContext.Provider value={resources}>
      <Drawer />
    </ResourcesContext.Provider>
  );
}
