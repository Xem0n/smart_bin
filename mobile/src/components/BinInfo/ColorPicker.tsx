import { StyleSheet, View } from "react-native";
import { TouchableWithoutFeedback } from "react-native-gesture-handler";
import { PredefinedColors } from "../../constants";
import { ColorPickerProps } from "../../types/props";
import { useState } from "react";

export default function ColorPicker(props: ColorPickerProps) {
  const [colors, setColors] = useState(
    PredefinedColors.map((color, index) => (
      <TouchableWithoutFeedback
        key={index}
        style={{
          backgroundColor: color,
          borderWidth: color === props.current ? 1 : 3,
          ...styles.colorButton,
        }}
        onPress={() => updateColors(color)}
      />
    )),
  );

  const updateColors = (newColor: string) => {
    setColors(
      PredefinedColors.map((color, index) => (
        <TouchableWithoutFeedback
          key={index}
          style={{
            backgroundColor: color,
            borderWidth: color === newColor ? 1 : 3,
            ...styles.colorButton,
          }}
          onPress={() => updateColors(color)}
        />
      )),
    );

    props.update(newColor);
  };

  return <View style={styles.container}>{colors}</View>;
}

const styles = StyleSheet.create({
  container: {
    width: "65%",

    marginTop: 10,

    flexDirection: "row",
    justifyContent: "space-around",
    alignItems: "center",
  },
  colorButton: {
    width: 30,
    height: 30,

    borderColor: "#000",
    // borderWidth: 3,
    borderRadius: 50,
  },
});
