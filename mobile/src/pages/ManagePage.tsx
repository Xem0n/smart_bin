import { useContext } from "react";
import { StyleSheet, View } from "react-native";
import { ResourcesContext } from "../contexts";
import { BinInfo } from "../components/BinInfo";

export default function ManagePage() {
  const resources = useContext(ResourcesContext);

  const bins = resources?.bins.map((bin) => (
    <BinInfo key={bin.id} data={bin} />
  ));

  return (
    <View style={styles.container}>
      {bins}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    backgroundColor: "#fff",

    flex: 1,
    alignItems: "center",
    justifyContent: "flex-start",
    flexDirection: "column",
  },
});
