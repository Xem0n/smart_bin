import { StyleSheet, Text, View } from "react-native";
import { CategoryDisplayProps } from "../types/props";
import compareDate from "../utils/compare_date";

export default function CategoryDisplay(props: CategoryDisplayProps) {
  const style = styles(props.category.color);

  const amount = props.category.garbages.reduce(
    (acc, garbage) =>
      acc +
      (compareDate(props.date, new Date(garbage.created_at * 1000)) ? 1 : 0),
    0,
  );

  return (
    <View style={style.container}>
      <Text style={style.text}>{props.category.name}</Text>
      <Text style={[style.text, style.right]}>{amount}</Text>
    </View>
  );
}

const styles = (color: string) =>
  StyleSheet.create({
    container: {
      width: "80%",
      height: 100,

      marginVertical: 25,
      padding: 25,

      backgroundColor: color + "12",

      borderRadius: 10,
      borderWidth: 3,
      borderBottomWidth: 7,
      borderColor: color,

      alignItems: "center",
      justifyContent: "flex-start",
      flexDirection: "row",
    },
    text: {
      fontSize: 15,
      fontWeight: "bold",
    },
    right: {
      fontSize: 20,
      marginLeft: "auto",
      marginRight: 0,
    },
  });
