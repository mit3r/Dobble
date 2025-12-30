import { getIconsSource, quasiRandomRotation } from "@/utils/CardsSymbols";
import { useMemo } from "react";

export default function IconComponent(props: {
  cardId: number;
  iconId: number;
  minSize: number;
  maxSize: number;
  left: number;
  top: number;
}) {
  const src = useMemo(() => getIconsSource(props.iconId), [props.iconId]);
  const rotation = useMemo(
    () => quasiRandomRotation(props.cardId, props.iconId),
    [props.cardId, props.iconId]
  );

  const size = useMemo(() => {
    const sizeRange = props.maxSize - props.minSize;
    const pseudoRandomFactor = ((props.cardId + props.iconId) * 37) % 100;
    return props.minSize + (pseudoRandomFactor / 100) * sizeRange;
  }, [props.cardId, props.iconId, props.minSize, props.maxSize]);

  return (
    <div
      className="absolute aspect-square  transition-transform"
      style={{
        width: `${size}%`,
        height: `${size}%`,
        left: `${props.left}%`,
        top: `${props.top}%`,
        transform: `translate(-50%, -50%) rotate(${rotation}deg)`,
      }}
    >
      <img
        draggable={false}
        className=" w-full h-full object-contain hover:scale-110 transition-transform"
        src={src}
        alt={`Icon ${props.iconId}`}
      />
    </div>
  );
}
