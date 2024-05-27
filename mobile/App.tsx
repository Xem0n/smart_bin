import "react-native-gesture-handler";
import { useCallback, useEffect, useState } from "react";
import { Resources, fetchResources } from "./src/api/resources";
import { ActiveBinContext, ResourcesContext } from "./src/contexts";
import Drawer from "./src/navigation/drawer/Drawer";
import { RefreshControl, ScrollView } from "react-native";

export default function App() {
  const [resources, setResources] = useState<Resources>();
  const [activeBin, setActiveBin] = useState<string | undefined>();
  const [refreshing, setRefreshing] = useState<boolean>(false);

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

  const onRefresh = useCallback(() => {
    setRefreshing(true);

    fetchData()
      .then((resources) => setActiveBin(resources?.bins[0].id))
      .catch(console.error)
      .finally(() => setRefreshing(false));
  }, []);

  return (
    <ResourcesContext.Provider value={{ data: resources, update: fetchData }}>
      <ActiveBinContext.Provider
        value={{ data: activeBin, update: setActiveBin }}
      >
        <ScrollView
          contentContainerStyle={{ flexGrow: 1 }}
          refreshControl={
            <RefreshControl refreshing={refreshing} onRefresh={onRefresh} />
          }
        >
          <Drawer />
        </ScrollView>
      </ActiveBinContext.Provider>
    </ResourcesContext.Provider>
  );
}
