import { StyleSheet, View } from "react-native";
import CategoryDisplay from "../components/CategoryDisplay";
import { useContext, useState } from "react";
import { ResourcesContext } from "../contexts";
import { DatePicker } from "../components/DatePicker";

export default function DataPage() {
  const [date, setDate] = useState(new Date());
  const resources = useContext(ResourcesContext);

  const categories = resources?.data?.bins[0].categories.map((category) => (
    <CategoryDisplay key={category.id} category={category} date={date} />
  ));

  return (
    <View style={styles.container}>
      <DatePicker date={date} updateDate={setDate} />
      <View style={styles.categories}>{categories}</View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    backgroundColor: "#fff",

    flex: 1,
    alignItems: "center",
    justifyContent: "flex-start",
    flexDirection: "column",
  },
  categories: {
    width: "100%",
    backgroundColor: "#F3F3F3",

    marginTop: 20,
    paddingTop: 25,

    borderColor: "#7a7a7a",
    borderWidth: 1,
    borderTopLeftRadius: 14,
    borderTopRightRadius: 14,

    flex: 1,
    alignItems: "center",
    justifyContent: "flex-start",
    flexDirection: "column",
  },
});
