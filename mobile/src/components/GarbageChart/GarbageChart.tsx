import { StyleSheet, Text, View } from "react-native";
import { LineChart } from "react-native-gifted-charts";
import { GarbageChartProps } from "../../types/props";

function numberWithLeadingZero(num: number) {
  return num.toString().padStart(2, "0");
}

// todo:
//  style the chart

export default function GarbageChart(props: GarbageChartProps) {
  const datasets =
    props.data?.categories.map((category) => {
      const values = [...Array(24).keys()].map(() => ({
        value: Math.floor(Math.random() * 15),
      }));

      for (const garbage of category.garbages) {
        const hour = new Date(garbage.created_at).getHours();
        values[hour].value++;
      }

      return values;
    }) ?? [];

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
          scrollToIndex={new Date().getHours()}
          xAxisLabelTexts={Array.from(
            { length: 24 },
            (_, i) => numberWithLeadingZero(i) + ":00",
          )}
        />
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 3,
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

    marginTop: 50,
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
});
