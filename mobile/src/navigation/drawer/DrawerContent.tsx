import {
  DrawerContentComponentProps,
  DrawerContentScrollView,
  DrawerItemList,
} from "@react-navigation/drawer";
import { Text } from "react-native";
import { StyleSheet } from "react-native";

export default function DrawerContent(props: DrawerContentComponentProps) {
  return (
    <DrawerContentScrollView {...props}>
      <Text style={styles.title}>Odkrywaj</Text>
      <DrawerItemList {...props} />
    </DrawerContentScrollView>
  );
}

const styles = StyleSheet.create({
  title: {
    fontSize: 18,
    marginLeft: 12,
    marginVertical: 12,
    fontWeight: "bold",
  },
});
