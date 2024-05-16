import { createDrawerNavigator } from "@react-navigation/drawer";
import { NavigationContainer } from "@react-navigation/native";
import DrawerContent from "./DrawerContent";
import { Feather } from "@expo/vector-icons";
import { StyleSheet } from "react-native";
import HeaderLogo from "./HeaderLogo";
import ManagePage from "../../pages/ManagePage";
import HomePage from "../../pages/HomePage";
import DataPage from "../../pages/DataPage";

const Navigation = createDrawerNavigator();

export default function Drawer() {
  return (
    <NavigationContainer>
      <Navigation.Navigator
        drawerContent={DrawerContent}
        initialRouteName="Home"
        screenOptions={{
          headerTitle: "",
          headerRight: () => <HeaderLogo />,
          drawerType: "front",
          headerTintColor: "#000",
          drawerActiveTintColor: "#000",
          drawerActiveBackgroundColor: "#efefef",
          drawerItemStyle: styles.drawerItem,
        }}
      >
        <Navigation.Screen
          name="Home"
          component={HomePage}
          options={{
            drawerIcon: () => <Feather name="home" size={24} color="black" />,
          }}
        />
        <Navigation.Screen
          name="Dane"
          component={DataPage}
          options={{
            drawerIcon: () => (
              <Feather name="bar-chart-2" size={24} color="black" />
            ),
          }}
        />
        <Navigation.Screen
          name="SmartBin"
          component={ManagePage}
          options={{
            drawerIcon: () => <Feather name="user" size={24} color="black" />,
          }}
        />
      </Navigation.Navigator>
    </NavigationContainer>
  );
}

const styles = StyleSheet.create({
  drawerItem: {
    // marginLeft: 0,
    // paddingLeft: 10,
    // borderColor: "#aaa",
    // borderTopWidth: 1,
    // borderRightWidth: 4,
    // borderBottomWidth: 4,
    // borderRadius: 10,
    // borderTopLeftRadius: 0,
    // borderBottomLeftRadius: 0,
  },
});
