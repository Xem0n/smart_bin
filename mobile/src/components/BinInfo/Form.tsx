import { useContext, useRef, useState } from "react";
import { Platform, StyleSheet, Text, TextInput, View } from "react-native";
import { TouchableOpacity } from "react-native-gesture-handler";
import { FormProps } from "../../types/props";
import ColorPicker from "./ColorPicker";
import { BinData } from "../../api/resources";
import { ActiveBinContext } from "../../contexts";

export default function Form(props: FormProps) {
  const activeBin = useContext(ActiveBinContext);
  const updatedData = useRef<BinData>({ ...props.data });
  const [name, setName] = useState<string>(props.data.name);

  const isSelected = props.data.id === activeBin?.data;

  const updateName = (name: string) => {
    updatedData.current.name = name;
    setName(name);
  };

  const updateColor = (color: string) => (updatedData.current.color = color);

  return (
    <View style={styles.container}>
      <Text style={styles.label}>Zmień nazwę</Text>
      <TextInput
        style={styles.nameInput}
        value={name}
        onChangeText={updateName}
      />

      <Text style={styles.label}>Zmień kolor</Text>

      <ColorPicker current={props.data.color} update={updateColor} />

      <View style={styles.buttonRow}>
        <TouchableOpacity
          style={styles.button}
          onPress={() => props.update(updatedData.current)}
        >
          <Text>Zapisz</Text>
        </TouchableOpacity>

        <TouchableOpacity
          style={styles.button}
          onPress={() => activeBin?.update(props.data.id)}
          disabled={isSelected}
        >
          <Text style={{ color: isSelected ? "#838383" : "#000" }}>
            {isSelected ? "Wybrany" : "Wybierz"}
          </Text>
        </TouchableOpacity>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    backgroundColor: "#f3f3f3",
    zIndex: -1,
    elevation: Platform.OS === "android" ? -1 : 0,

    width: "100%",

    marginTop: -10,
    paddingTop: 10,
    paddingBottom: 10,

    borderRadius: 12,

    shadowColor: "#83838383",
    shadowOffset: {
      width: 0,
      height: 0,
    },
    shadowRadius: 5,
    shadowOpacity: 1,

    alignItems: "center",
  },
  label: {
    marginTop: 15,
  },
  nameInput: {
    width: "70%",

    marginTop: 10,
    paddingVertical: 5,

    borderBottomWidth: 1,
    borderColor: "#000",

    textAlign: "center",
  },
  buttonRow: {
    width: "62%",

    flexDirection: "row",
    justifyContent: "space-between",
  },
  button: {
    backgroundColor: "#83838355",

    marginTop: 20,

    paddingHorizontal: 25,
    paddingVertical: 5,

    borderColor: "#83838383",
    borderRadius: 6,
  },
});
