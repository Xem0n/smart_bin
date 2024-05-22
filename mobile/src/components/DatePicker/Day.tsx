import { StyleSheet, Text } from "react-native";
import { DayProps } from "../../types/props";
import { ShortWeekDays } from "../../constants";
import modulo from "../../utils/modulo";
import { TouchableWithoutFeedback } from "react-native-gesture-handler";

export default function Day(props: DayProps) {
  const weekDay =
    ShortWeekDays[modulo(props.date.getDay() - 1, ShortWeekDays.length)];

  return (
    <TouchableWithoutFeedback
      onPress={() => props.select()}
      style={[styles.container, props.selected ? styles.selected : {}]}
    >
      <Text style={styles.weekDay}>{weekDay}</Text>
      <Text style={styles.day}>{props.date.getDate()}</Text>
    </TouchableWithoutFeedback>
  );
}

const styles = StyleSheet.create({
  container: {
    width: 50,
    height: 70,

    marginHorizontal: 5,

    borderRadius: 10,

    flexDirection: "column",
    alignItems: "center",
    justifyContent: "space-evenly",
  },
  selected: {
    backgroundColor: "#F5F5F5",

    borderWidth: 1,
    borderBottomWidth: 3,
    borderColor: "#0000003a",
  },
  weekDay: {},
  day: {
    fontWeight: "bold",
    fontSize: 15,
  },
});
