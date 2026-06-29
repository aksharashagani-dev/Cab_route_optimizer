const express = require("express");
const { execFile } = require("child_process");
const path = require("path");

const app = express();
const PORT = 3000;
const BINARY = path.join(__dirname, "cab_route");

app.use(express.static(path.join(__dirname, "public")));

// GET /route?from=X&to=Y&hour=9
app.get("/route", (req, res) => {
  const { from, to, hour } = req.query;

  if (!from || !to) {
    return res.status(400).json({ error: "Missing required params: from, to" });
  }

  const hourArg = hour !== undefined ? String(parseInt(hour)) : String(new Date().getHours());

  execFile(BINARY, [from, to, hourArg], (err, stdout, stderr) => {
    if (err) {
      return res.status(500).json({ error: "Binary execution failed", details: err.message });
    }
    try {
      const result = JSON.parse(stdout.trim());
      if (result.error) return res.status(404).json(result);
      return res.json(result);
    } catch (e) {
      return res.status(500).json({ error: "Failed to parse binary output", raw: stdout });
    }
  });
});

app.listen(PORT, () => console.log(`Server running at http://localhost:${PORT}`));