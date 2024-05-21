import { StyleSheet, View } from "react-native";
import { GarbageChart } from "../components/GarbageChart";
import { useContext } from "react";
import { ActiveBinContext, ResourcesContext } from "../contexts";

export default function HomePage() {
  const resources = useContext(ResourcesContext);
  const activeBin = useContext(ActiveBinContext);

  const binData = resources?.data?.bins.find((bin) => bin.id === activeBin?.data);

  return (
    <View style={styles.container}>
      <View style={styles.idk} />
      <GarbageChart  data={binData} />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    backgroundColor: "#fff",

    flex: 1,
    alignItems: "center",
    justifyContent: "center",
  },
  idk: {
    flex: 2,
  }
});
