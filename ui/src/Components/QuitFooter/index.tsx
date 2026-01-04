export default function QuitFooter(props: { onQuit?: () => void; label?: string }) {
  return (
    <div className="w-full flex justify-end items-center p-4 rounded-xl bg-dobble-card/50">
      <button className="btn-danger" onClick={props.onQuit}>
        {props.label || "Quit"}
      </button>
    </div>
  );
}
