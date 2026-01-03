import { ConnectionStatusSpan } from "@/Components/Statuses";
import { ConnectionError, ConnectionStatus } from "@/types/dobble";
import { useMemo } from "react";
import { ConnectionErrorSpan } from "../Statuses";

export default function ServerStatus({
  connectionStatus,
  connectionError,
  retryCallback,
}: {
  connectionStatus: ConnectionStatus;
  connectionError: ConnectionError | null;
  retryCallback?: () => void;
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
      <div className="text-red-500 flex gap-4 items-baseline w-full justify-center">
        <span>{connectionErrorElement}</span>
        {retryCallback && (
          <button className="p-2 border-red-500 px-4" onClick={retryCallback}>
            Retry
          </button>
        )}
      </div>
    );

  if (connectionStatusElement)
    return (
      <div className="text-blue-500 flex justify-center w-full">
        {connectionStatusElement}
        {connectionStatus === ConnectionStatus.Disconnected && retryCallback && (
          <button className="p-2 border-blue-500 px-4 ml-4" onClick={retryCallback}>
            Reconnect
          </button>
        )}
      </div>
    );

  return <></>;
}
