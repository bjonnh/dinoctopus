import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App.tsx'
import {createTheme, CssBaseline, ThemeProvider} from "@mui/material";

const darkTheme = createTheme({
    palette: {
        mode: 'dark',
    },
});

ReactDOM.createRoot(document.getElementById('root')!).render(<React.StrictMode>
    <ThemeProvider theme={darkTheme}>
        <CssBaseline/>
        <App/>
    </ThemeProvider>
</React.StrictMode>,)
