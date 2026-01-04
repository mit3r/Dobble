import { motion } from "motion/react";

export default function AlertMistake() {
  return (
    <motion.span
      initial={{ opacity: 0, scale: 0.8 }}
      animate={{ opacity: 1, scale: 1 }}
      exit={{ opacity: 0, scale: 0.8 }}
      transition={{ duration: 0.3 }}
      className="text-red-500 font-bold text-2xl"
    >
      Mistake!
    </motion.span>
  );
}
