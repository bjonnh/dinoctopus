import {DeviceProvider} from "./providers/DeviceProvider";
import Main from "./components/Main";
import {Box, Container, Typography} from "@mui/material";

function App() {
    return (
        <Container maxWidth="lg">
            <Box sx={{ my: 4 }}>
                <Typography variant="h4" component="h1" gutterBottom>
                    DINOctopus
                </Typography>
                <DeviceProvider>
                    <Main></Main>
                </DeviceProvider>
                {/* Your content here */}
            </Box>
        </Container>
    )
}

export default App
