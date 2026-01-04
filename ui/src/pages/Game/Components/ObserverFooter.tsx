export default function ObserverFooter() {
  return (
    <div className="w-full flex justify-between items-center gap-4 p-4 rounded-xl bg-dobble-card/50">
      <div className="flex-1 flex justify-center items-center gap-2">
        <span className="text-xl text-dobble-text-muted">You are observing the game</span>
      </div>

      <button 
        className="btn-danger" 
        onClick={() => window.bridges?.game.callQuitGame()}
      >
        Leave
      </button>
    </div>
  );
}
