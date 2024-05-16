import { ScrollView, StyleSheet } from "react-native";
import { MonthProps } from "../../types/props";
import Day from "./Day";
import { useEffect, useRef } from "react";

export default function Calendar(props: MonthProps) {
  const scrollView = useRef<ScrollView>(null);

  const daysInMonth = new Date(
    props.date.getFullYear(),
    props.date.getMonth() + 1,
    0,
  ).getDate();

  const updateDay = (day: number) =>
    props.updateDate(new Date(props.date.setDate(day)));

  const days = Array.from({ length: daysInMonth }, (_, i) => i + 1).map(
    (day) => (
      <Day
        key={day}
        date={new Date(props.date.getFullYear(), props.date.getMonth(), day)}
        selected={day === props.date.getDate()}
        select={() => updateDay(day)}
      />
    ),
  );

  const autoScroll = () => {
    scrollView.current?.scrollTo({
      x: 50 * (props.date.getDate() - 1),
      animated: true,
    });
  };

  useEffect(() => {
    autoScroll();
  }, [props.date]);

  return (
    <ScrollView
      ref={scrollView}
      onLayout={() => setTimeout(autoScroll, 100)}
      style={styles.container}
      horizontal
      showsHorizontalScrollIndicator={false}
    >
      {days}
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    marginTop: 10,
  },
});
