import Dots from "@/Components/Dots";

export default function LoadingLobby() {
  return (
    <div className="panel h-full flex flex-col items-center justify-center">
      <div className="animate-pulse text-center">
        <h1 className="text-2xl font-bold text-dobble-text flex items-center justify-center">
          Loading lobby
          <Dots />
        </h1>
        <p className="text-dobble-text-muted mt-2">Please wait...</p>
      </div>
    </div>
  );
}