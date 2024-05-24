import { LinearGradient } from "expo-linear-gradient";
import { StyleSheet, Text, View } from "react-native";
import { AmountDisplayProps } from "../../types/props";
import compareDate from "../../utils/compare_date";

export default function AmountDisplay(props: AmountDisplayProps) {
  const currentDay = new Date();
  console.log(currentDay.getHours());
  const amount = props.data?.categories.reduce(
    (sum, category) =>
      sum +
      category.garbages.filter((garbage) =>
        compareDate(new Date(garbage.created_at * 1000), currentDay),
      ).length,
    0,
  );

  return (
    <LinearGradient
      colors={["#5aa9e7", "#f1454b", "#e6ab17"]}
      start={{ x: 0.5, y: 0 }}
      end={{ x: 0, y: 1 }}
      style={styles.container}
    >
      <View style={styles.inner}>
        <Text style={styles.amount}>{amount}</Text>
        <Text style={styles.label}>Śmieci</Text>
      </View>
    </LinearGradient>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 2,
    aspectRatio: 1,

    borderRadius: 150,
    marginVertical: 30,
  },
  inner: {
    borderRadius: 150,

    margin: 10,

    backgroundColor: "#fff",

    flex: 1,
    alignItems: "center",
    justifyContent: "center",
    flexDirection: "column",
  },
  amount: {
    fontSize: 50,
  },
  label: {
    fontSize: 20,

    marginTop: 15,
  },
});
