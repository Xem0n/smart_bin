import { StyleSheet, Text, View } from "react-native";
import Arrow from "./Arrow";
import Calendar from "./Calendar";
import { DatePickerProps } from "../../types/props";
import { Months, FullWeekDays } from "../../constants";
import modulo from "../../utils/modulo";

export default function DatePicker(props: DatePickerProps) {
  const monthName = Months[props.date.getMonth()];
  const weekDay =
    FullWeekDays[modulo(props.date.getDay() - 1, FullWeekDays.length)];

  const updateMonth = (offset: number) => {
    const month = modulo(props.date.getMonth() + offset, Months.length);
    const monthDiff = month - props.date.getMonth();
    let year = props.date.getFullYear();

    if (Math.abs(monthDiff) === 11) {
      year -= Math.sign(monthDiff);
    }

    props.updateDate(new Date(year, month, props.date.getDate()));
  };

  return (
    <View style={styles.container}>
      <View style={styles.month}>
        <Arrow onPress={() => updateMonth(-1)} direction="left" />
        <Text style={styles.monthText}>{monthName}</Text>
        <Arrow onPress={() => updateMonth(1)} direction="right" />
      </View>

      <Calendar {...props} />

      <Text style={styles.date}>
        {weekDay}, {props.date.getDate()} {monthName} {props.date.getFullYear()}
      </Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    width: "100%",

    paddingTop: 10,
    paddingHorizontal: 10,
  },
  month: {
    alignItems: "center",
    justifyContent: "space-between",
    flexDirection: "row",
  },
  monthText: {
    fontSize: 24,
    fontWeight: "bold",
  },
  date: {
    marginTop: 30,

    fontSize: 18,
    fontWeight: "bold",
    textAlign: "center",
  },
});
