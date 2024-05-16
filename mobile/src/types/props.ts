import React from "react";
import { GarbageCategory } from "../api/resources";

export interface DatePickerProps {
  date: Date;
  updateDate: React.Dispatch<React.SetStateAction<Date>>;
}

export interface ArrowProps {
  direction: "left" | "right";
  onPress: () => void;
}

export interface MonthProps extends DatePickerProps {}

export interface DayProps {
  selected: boolean;
  date: Date;
  select: () => void;
}

export interface CategoryDisplayProps {
  category: GarbageCategory;
  date: Date;
}
