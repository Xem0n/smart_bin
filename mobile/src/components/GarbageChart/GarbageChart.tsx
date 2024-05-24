import { StyleSheet, Text, View } from "react-native";
import { LineChart } from "react-native-gifted-charts";
import { GarbageChartProps } from "../../types/props";
import compareDate from "../../utils/compare_date";

function numberWithLeadingZero(num: number) {
  return num.toString().padStart(2, "0");
}

export default function GarbageChart(props: GarbageChartProps) {
  const currentDay = new Date();

  const datasets =
    props.data?.categories.map((category) => {
      const values = [...Array(24).keys()].map(() => ({
        value: 0,
      }));

      for (const garbage of category.garbages) {
        const date = new Date(garbage.created_at * 1000);

        if (!compareDate(date, currentDay)) {
          continue;
        }

        const hour = date.getHours();
        values[hour].value++;
      }

      return values;
    }) ?? [];

  const maxValue = Math.max(
    ...datasets.flat().flatMap((dataset) => dataset.value ?? 0),
  );

  return (
    <View style={styles.container}>
      <Text style={styles.title}>Wykaz śmieci - dziś</Text>

      <View style={styles.chart}>
        <LineChart
          width={260}
          data={datasets[0]}
          data2={datasets[1]}
          data3={datasets[2]}
          color1={props.data?.categories[0].color}
          color2={props.data?.categories[1].color}
          color3={props.data?.categories[2].color}
          maxValue={maxValue}
          hideRules
          hideDataPoints
          curved
          scrollToIndex={new Date().getHours()}
          xAxisLabelTexts={Array.from(
            { length: 24 },
            (_, i) => numberWithLeadingZero(i) + ":00",
          )}
        />

        <View style={styles.legend}>
          {props.data?.categories.map((category, index) => (
            <Text key={index} style={{ color: category.color }}>
              {category.name}
            </Text>
          ))}
        </View>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 4,
    alignItems: "center",
    justifyContent: "flex-start",
    flexDirection: "column",

    backgroundColor: "#F3F3F3",

    borderRadius: 24,

    width: "100%",

    shadowColor: "#83838383",
    shadowOffset: {
      width: 0,
      height: 0,
    },
    shadowRadius: 5,
    shadowOpacity: 1,
  },
  title: {
    fontSize: 16,
    color: "#828282",

    marginTop: 40,
  },
  chart: {
    backgroundColor: "#fff",

    alignItems: "center",
    justifyContent: "center",

    width: 320,
    marginTop: 15,
    paddingVertical: 10,
    borderRadius: 12,
  },
  legend: {
    width: 200,

    marginTop: 15,

    flexDirection: "row",
    justifyContent: "space-around",
    alignItems: "center",
  },
});
