import GarbageCategory from "./garbage_category";

export default interface BinData {
  id: string;
  name: string;
  color: string;
  categories: GarbageCategory[];
}
