import React from "react";
import { BinData, GarbageCategory } from "../api/resources";

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

export interface BinInfoProps {
  data: BinData;
  update: () => void;
}

export interface FormProps {
  data: BinData;
  update: (data: BinData) => void;
}

export interface ColorPickerProps {
  current: string;
  update: (color: string) => void;
}
