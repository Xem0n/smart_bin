import { Feather } from "@expo/vector-icons";
import { TouchableOpacity } from "react-native-gesture-handler";
import { ArrowProps } from "../../types/props";

export default function Arrow(props: ArrowProps) {
  const iconName = `chevron-${props.direction}` as "chevron-left" | "chevron-right";

  return (
    <TouchableOpacity onPress={props.onPress}>
      <Feather name={iconName} size={40} />
    </TouchableOpacity>
  );
}
