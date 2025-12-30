import { useEffect, useMemo, useState } from "react";

export default function Dots() {
  const [value, setValue] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => setValue((prev) => (prev + 1) % 4), 500);
    return () => clearInterval(interval);
  }, []);

  const dots = useMemo(() => {
    return ".".repeat(value);
  }, [value]);

  return <span>{dots}</span>;
}