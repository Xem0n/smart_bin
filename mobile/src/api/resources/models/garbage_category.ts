import GarbageData from "./garbage_data";

export default interface GarbageCategory {
  id: string;
  name: string;
  color: string;
  garbages: GarbageData[];
}
