import {DeviceState} from "../../interfaces/DeviceState.tsx"

function Matrix({ deviceState, onUpdate, disabled }: { deviceState: DeviceState, onUpdate: (index: number, newValue: number) => void, disabled: boolean }) {
    const gridSize = Math.sqrt(deviceState.size) // it's better be square

    const renderCheckbox = (index: number) => (
        <input
            type="checkbox"
            checked={deviceState.matrix[index] == 1}
            disabled={disabled}
            onChange={(e) => onUpdate(index, e.target.checked ? 1 : 0)}
            key={`checkbox-${index}`}
        />
    );

    const renderRowLabel = (rowIndex: number) => (
        <div key={`row-label-${rowIndex}`} style={{ gridColumn: '1', gridRow: rowIndex + 2 }}>
            {`MIDI In ${rowIndex + 1}`}
        </div>
    );

    const renderColumnLabel = (colIndex: number) => (
        <div key={`col-label-${colIndex}`} style={{ gridColumn: colIndex + 2, gridRow: '1' }}>
            {`MIDI Out ${colIndex + 1}`}
        </div>
    );

    return (
        <div style={{ display: 'grid', gridTemplateColumns: `auto repeat(${gridSize}, auto)`, gridTemplateRows: `auto repeat(${gridSize}, auto)`, gap: '10px' }}>
            {[...Array(gridSize).keys()].map(renderColumnLabel)}
            {[...Array(gridSize).keys()].map(renderRowLabel)}
            {deviceState.matrix.map((_, index) => (
                <div key={`cell-${index}`} style={{ gridColumn: (index % gridSize) + 2, gridRow: Math.floor(index / gridSize) + 2 }}>
                    {renderCheckbox(index)}
                </div>
            ))}
        </div>
    );
}

export default Matrix
