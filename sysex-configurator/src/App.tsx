import {DeviceProvider} from "./providers/DeviceProvider";
import Main from "./components/Main";
import {Box, Container, Typography} from "@mui/material";
import myImage from "../public/logo.svg"

function App() {
    return (
        <Container maxWidth="lg">
            <Box sx={{my: 4}}>
                <Typography variant="h4" component="h1" gutterBottom>
                    DINOctopus
                </Typography>
                <img src={myImage} alt="Logo" width={"10%"}/>
                <DeviceProvider>
                    <Main></Main>
                </DeviceProvider>
            </Box>
        </Container>
    )
}

export default App
