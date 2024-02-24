function Matrix({ deviceState, onCheckboxChange, disabled }: { deviceState: DeviceState, onCheckboxChange: (index: number, newValue: number) => void, disabled: boolean }) {
    const gridSize = 5; // Assuming a 5x5 matrix for simplicity, adjust as necessary

    const renderCheckbox = (index: number) => (
        <input
            type="checkbox"
            checked={deviceState.matrix[index] == 1}
            disabled={disabled}
            onChange={(e) => onCheckboxChange(index, e.target.checked ? 1 : 0)}
            key={`checkbox-${index}`}
        />
    );

    const renderRowLabel = (rowIndex: number) => (
        <div key={`row-label-${rowIndex}`} style={{ gridColumn: '1', gridRow: rowIndex + 2 }}>
            {`MIDI ${rowIndex + 1}`}
        </div>
    );

    const renderColumnLabel = (colIndex: number) => (
        <div key={`col-label-${colIndex}`} style={{ gridColumn: colIndex + 2, gridRow: '1' }}>
            {`MIDI ${colIndex + 1}`}
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
