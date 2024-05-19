import { Platform, StyleSheet, Text, View } from "react-native";
import { BinInfoProps } from "../../types/props";
import Form from "./Form";
import { useState } from "react";
import { TouchableWithoutFeedback } from "react-native-gesture-handler";
import { Feather } from "@expo/vector-icons";
import { BinData } from "../../api/resources";

export default function BinInfo(props: BinInfoProps) {
  const [data, setData] = useState<BinData>(props.data);
  const [expanded, setExpanded] = useState<boolean>(false);

  // todo: add animation for expanding

  const update = (newData: BinData) => {
    setData(newData);
    setExpanded(false);

    // todo: send update request

    props.update();
  };

  return (
    <View style={styles.container}>
      <TouchableWithoutFeedback
        style={{ ...styles.header, backgroundColor: data.color }}
        onPress={() => setExpanded(!expanded)}
      >
        <Text style={styles.name}>{data.name}</Text>
        <Feather name="chevron-down" size={32} />
      </TouchableWithoutFeedback>

      {expanded && <Form data={data} update={update} />}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    width: "90%",
    height: "auto",
    marginTop: 30,
  },
  header: {
    zIndex: 99,
    elevation: Platform.OS === "android" ? 99 : 0,

    width: "100%",
    height: 80,
    paddingHorizontal: 20,

    borderWidth: 1,
    borderColor: "#E5E5E5",
    borderRadius: 12,

    shadowColor: "#83838383",
    shadowOffset: {
      width: 0,
      height: 0,
    },
    shadowRadius: 5,
    shadowOpacity: 1,

    alignItems: "center",
    justifyContent: "space-between",
    flexDirection: "row",
  },
  name: {
    fontSize: 18,
  },
});
