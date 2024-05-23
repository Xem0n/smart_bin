const URL = process.env.WEBSERVER_URL ?? "http://192.168.109.103:5000";

const Months = [
    "Styczeń",
    "Luty",
    "Marzec",
    "Kwiecień",
    "Maj",
    "Czerwiec",
    "Lipiec",
    "Sierpień",
    "Wrzesień",
    "Październik",
    "Listopad",
    "Grudzień",
];

const PredefinedColors = [
    "#e5b514",
    "#5aa9e7",
    "#f94144",
    "#fa34d5",
    "#f3f3f3",
];

const FullWeekDays = [
    "Poniedziałek",
    "Wtorek",
    "Środa",
    "Czwartek",
    "Piątek",
    "Sobota",
    "Niedziela",
];

const ShortWeekDays = ["Pon", "Wt", "Śr", "Czw", "Pt", "Sob", "Niedz"];

export { URL, Months, PredefinedColors, FullWeekDays, ShortWeekDays };
