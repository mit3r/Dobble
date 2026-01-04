import { motion } from "motion/react";

export default function AlertGoodMatch() {
  return (
    <motion.span
      initial={{ opacity: 0, scale: 0.8 }}
      animate={{ opacity: 1, scale: 1 }}
      exit={{ opacity: 0, scale: 0.8 }}
      transition={{ duration: 0.3 }}
      className="text-dobble-success font-bold text-2xl"
    >
      Good Match!
    </motion.span>
  );
}
