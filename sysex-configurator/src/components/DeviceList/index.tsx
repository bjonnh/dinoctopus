import {Accordion, AccordionDetails, AccordionSummary, Typography} from "@mui/material";
import ExpandMoreIcon from '@mui/icons-material/ExpandMore';


function DeviceAccordion({midiInputs, midiOutputs, handleDeviceClick, selectedDevice}) {
    return (<div>
        <Accordion>
            <AccordionSummary expandIcon={<ExpandMoreIcon/>}>
                <Typography>
                    {selectedDevice.input && selectedDevice.output && (selectedDevice.input.name == selectedDevice.output.name && (
                            <div>Selected device: {selectedDevice.input.name}</div>) || (
                            <div>Selected input: {selectedDevice.input.name} - Selected
                                output: {selectedDevice.output.name}</div>))}
                </Typography>
            </AccordionSummary>
            <AccordionDetails>
                <div style={{display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '4em'}}>
                    <ul>
                        {midiInputs.map((input, index) => (<li key={index} onClick={() => handleDeviceClick(input.name)}
                                                               style={{
                                                                   cursor: 'pointer',
                                                                   fontWeight: selectedDevice.input && selectedDevice.input.name === input.name ? 'bold' : 'normal'
                                                               }}
                        >
                            {input.name}
                        </li>))}
                    </ul>
                    <ul>
                        {midiOutputs.map((output, index) => (
                            <li key={index} onClick={() => handleDeviceClick(output.name)}
                                style={{
                                    cursor: 'pointer',
                                    fontWeight: selectedDevice.output && selectedDevice.output.name === output.name ? 'bold' : 'normal'
                                }}
                            >
                                {output.name}
                            </li>))}
                    </ul>
                </div>
            </AccordionDetails>
        </Accordion>
    </div>);
}

export default DeviceAccordion
