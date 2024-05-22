import { URL } from "../../constants";
import BinData from "./models/bin_data";

export default async function updateBinInfo(data: BinData) {
  await fetch(`${URL}/update`, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({
      id: data.id,
      name: data.name,
      color: data.color,
    }),
  });
}
