import { ConnectionStatusSpan } from "@/Components/Statuses";
import { ConnectionError, ConnectionStatus } from "@/types/dobble";
import { useMemo } from "react";
import { ConnectionErrorSpan } from "../Statuses";

export default function ServerStatus({
  connectionStatus,
  connectionError,
  retryCallback,
  editCallback,
}: {
  connectionStatus: ConnectionStatus;
  connectionError: ConnectionError | null;
  retryCallback?: () => void;
  editCallback?: () => void;
}) {
  window.bridges?.main.callMsg(
    "LobbyServerConnectionStatus - connectionStatus: " +
      connectionStatus +
      ", connectionError: " +
      connectionError
  );

  const connectionStatusElement = useMemo(() => {
    if (connectionStatus === ConnectionStatus.Connected) return null;
    return <ConnectionStatusSpan status={connectionStatus} />;
  }, [connectionStatus]);

  const connectionErrorElement = useMemo(() => {
    if (connectionError === null || connectionError === ConnectionError.None) return null;
    return <ConnectionErrorSpan error={connectionError} />;
  }, [connectionError]);

  if (connectionErrorElement)
    return (
      <div className="flex gap-4 items-center justify-center w-full p-3 rounded-xl bg-dobble-danger/10">
        <span>{connectionErrorElement}</span>
        {retryCallback && (
          <button className="btn-primary py-2 px-4 text-sm" onClick={retryCallback}>
            Retry
          </button>
        )}
        {editCallback && (
          <button className="btn-secondary py-2 px-4 text-sm" onClick={editCallback}>
            Edit
          </button>
        )}
      </div>
    );

  if (connectionStatusElement)
    return (
      <div className="flex gap-4 items-center justify-center w-full p-3 rounded-xl bg-dobble-primary/10">
        {connectionStatusElement}
        {connectionStatus === ConnectionStatus.Disconnected && retryCallback && (
          <button className="btn-secondary py-2 px-4 text-sm" onClick={retryCallback}>
            🔄 Reconnect
          </button>
        )}
      </div>
    );

  return <></>;
}
