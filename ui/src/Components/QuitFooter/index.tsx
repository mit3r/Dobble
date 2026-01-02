export default function QuitFooter(props: { onQuit: () => void; label?: string }) {
  return (
    <div className="w-full flex justify-end items-center border-t-2 p-4">
      <button className="p-2 px-8 border-2 font-bold">{props.label || "Quit"}</button>
    </div>
  );
}
