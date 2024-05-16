import { useState } from "react";
import { Platform, StyleSheet, Text, TextInput, View } from "react-native";
import { TouchableOpacity } from "react-native-gesture-handler";
import { BinInfoProps } from "../../types/props";

export default function Form(props: BinInfoProps) {
  const [name, setName] = useState<string>(props.data.name);

  // todo: add color picker
  // todo: add select button

  return (
    <View style={styles.container}>
      <Text style={styles.label}>Zmień nazwę</Text>
      <TextInput style={styles.nameInput} value={name} onChangeText={setName} />

      <Text style={styles.label}>Zmień kolor</Text>

      <TouchableOpacity style={styles.button}>
        <Text>Zapisz</Text>
      </TouchableOpacity>
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
  button: {
    backgroundColor: "#83838355",

    marginTop: 20,
    paddingHorizontal: 25,
    paddingVertical: 5,

    borderColor: "#83838383",
    borderRadius: 6,
  },
});
