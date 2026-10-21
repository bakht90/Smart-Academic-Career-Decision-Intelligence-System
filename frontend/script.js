// SMART ACADEMIC & CAREER DECISION INTELLIGENCE SYSTEM
// Frontend -> C++ HTTP API -> C++ OOP -> TXT files

const API_BASE = window.location.protocol === "file:" ? "http://localhost:18080" : "";
const SESSION_KEY = "smartAcademicUsername";
const LEGACY_USER_KEY = "smartAcademicUser";
const LEGACY_ASSIGNMENTS_KEY = "smartAcademicAssignments";

const $ = (selector) => document.querySelector(selector);
const $$ = (selector) => Array.from(document.querySelectorAll(selector));

const loginPage = $("#loginPage");
const loginCard = $("#loginCard");
const registerCard = $("#registerCard");
const dashboardPage = $("#dashboardPage");
const loginForm = $("#loginForm");
const registerForm = $("#registerForm");
const loginMessage = $("#loginMessage");
const registerMessage = $("#registerMessage");

function setMessage(element, message, success = false) {
    if (!element) return;
    element.textContent = message;
    element.style.color = success ? "#8bd3a8" : "#e6a0a0";
}

async function api(path, options = {}) {
    const response = await fetch(API_BASE + path, {
        ...options,
        headers: {
            "Content-Type": "application/json",
            ...(options.headers || {})
        }
    });

    const data = await response.json().catch(() => ({
        success: false,
        message: "Invalid server response."
    }));

    if (!response.ok || data.success === false) {
        throw new Error(data.message || "Request failed.");
    }

    return data;
}

function currentUsername() {
    return sessionStorage.getItem(SESSION_KEY);
}

function setCurrentUsername(username) {
    if (username) sessionStorage.setItem(SESSION_KEY, username);
    else sessionStorage.removeItem(SESSION_KEY);
}

function showLogin() {
    if (loginPage) loginPage.style.display = "grid";
    if (loginCard) loginCard.style.display = "block";
    if (registerCard) registerCard.classList.remove("show-register");
    if (dashboardPage) dashboardPage.style.display = "none";
}

function showRegister() {
    if (loginPage) loginPage.style.display = "grid";
    if (loginCard) loginCard.style.display = "none";
    if (registerCard) registerCard.classList.add("show-register");
    if (dashboardPage) dashboardPage.style.display = "none";
}

function showDashboard() {
    if (loginPage) loginPage.style.display = "none";
    if (dashboardPage) dashboardPage.style.display = "grid";
}

async function getDashboard() {
    const username = currentUsername();
    if (!username) throw new Error("Please login first.");
    return api(`/api/dashboard?username=${encodeURIComponent(username)}`);
}

function applyDashboard(data) {
    if (!data || !data.user) return;

    const user = data.user;
    const pending = Number(data.pending || 0);
    const score = Number(data.performanceScore || 0);
    const match = Number(data.matchScore || 0);

    const studentName = $("#studentName");
    const pageTitle = $("#pageTitle");
    if (studentName) studentName.textContent = user.name;
    if (pageTitle) pageTitle.textContent = `Good morning, ${user.name}.`;

    // Profile
    const profileMap = {
        "#profileName": user.name,
        "#profileFullName": user.name,
        "#profileAge": user.age,
        "#profileDepartment": user.department,
        "#profileDepartmentValue": user.department,
        "#profileUsername": user.username,
        "#profileGpa": Number(user.gpa).toFixed(2),
        "#profileAttendance": `${user.attendance}%`
    };
    Object.entries(profileMap).forEach(([selector, value]) => {
        const element = $(selector);
        if (element) element.textContent = value;
    });
    const avatar = $("#profileAvatar");
    if (avatar) avatar.textContent = user.name ? user.name.charAt(0).toUpperCase() : "S";

    // Overview cards
    const cards = $$(".stats-grid .stat-card");
    if (cards.length >= 4) {
        cards[0].querySelector(".stat-value").textContent = Number(user.gpa).toFixed(2);
        cards[0].querySelector(".stat-note").textContent = Number(user.gpa) >= 3.5 ? "↑ Strong academic standing" : "Academic progress";

        cards[1].querySelector(".stat-value").textContent = `${user.attendance}%`;
        cards[1].querySelector(".stat-note").textContent = Number(user.attendance) >= 85 ? "↑ Above target" : "Needs attention";

        cards[2].querySelector(".stat-value").textContent = pending;
        cards[2].querySelector(".stat-note").textContent = pending === 1 ? "Assignment remaining" : "Assignments remaining";

        cards[3].querySelector(".stat-value").textContent = match ? `${match}%` : "—";
        cards[3].querySelector(".stat-note").textContent = data.career || "Complete assessment";
    }

    // Performance panel
    const scoreElement = $(".score-circle strong");
    if (scoreElement) scoreElement.textContent = score;
    const statusBadge = $(".performance-panel .status-badge");
    if (statusBadge) statusBadge.textContent = data.performanceStatus === "Poor Academic Performance" ? "NEEDS ATTENTION" : data.performanceStatus === "Average Academic Performance" ? "STABLE" : "ON TRACK";

    const barItems = $$(".performance-bars .bar-item");
    if (barItems.length >= 3) {
        const values = [
            Math.round((Number(user.gpa) / 4) * 100),
            Number(user.attendance),
            data.assignments && data.assignments.length ? Math.round(((data.assignments.length - pending) / data.assignments.length) * 100) : 100
        ];
        values.forEach((value, i) => {
            const strong = barItems[i].querySelector("strong");
            const fill = barItems[i].querySelector(".bar span");
            if (strong) strong.textContent = `${value}%`;
            if (fill) fill.style.width = `${Math.max(0, Math.min(100, value))}%`;
        });
    }

    updateAlerts(data.alerts || []);
    renderAssignments(data.assignments || []);

    // Career
    const careerTitle = $("#careerTitle");
    if (careerTitle) careerTitle.textContent = data.career || "Complete assessment";
    const careerPanel = $(".career-result");
    if (careerPanel) {
        const matchElement = careerPanel.querySelector(".career-match");
        const description = careerPanel.querySelector(".career-description");
        if (matchElement) matchElement.textContent = match ? `${match}% Match` : "No assessment yet";
        if (description) description.textContent = data.career ? `Your selected skill profile currently aligns with ${data.career.toLowerCase()}.` : "Select a skill to generate a rule-based career recommendation.";
    }

    // Academic update fields show current values.
    const gpaInput = $("#updateGpa");
    const attendanceInput = $("#updateAttendance");
    if (gpaInput) gpaInput.value = Number(user.gpa).toFixed(2);
    if (attendanceInput) attendanceInput.value = user.attendance;
}

function updateAlerts(alerts) {
    const list = $(".alert-list");
    if (!list) return;
    list.innerHTML = "";

    alerts.forEach((message) => {
        const item = document.createElement("div");
        item.className = "alert-item";
        const positive = message === "No Alerts.";
        item.innerHTML = `
            <div class="alert-icon ${positive ? "positive-icon" : "warning-icon"}">${positive ? "✓" : "!"}</div>
            <div>
                <strong>${positive ? "Everything looks good" : "Smart alert"}</strong>
                <p>${message}</p>
            </div>
        `;
        list.appendChild(item);
    });
}

function renderAssignments(assignments) {
    const container = $("#assignmentList");
    if (container) {
        container.innerHTML = "";
        if (!assignments.length) {
            container.innerHTML = `<div class="assignment-manager-row"><span>No assignments added yet.</span></div>`;
        } else {
            assignments.forEach((assignment) => {
                const row = document.createElement("div");
                row.className = "assignment-manager-row";
                const buttonClass = assignment.status === "Completed" ? "completed-btn" : "pending-btn";
                row.innerHTML = `<span></span><button type="button" class="assignment-status ${buttonClass}">${assignment.status}</button>`;
                row.querySelector("span").textContent = assignment.name;
                row.querySelector("button").addEventListener("click", async () => {
                    try {
                        const username = currentUsername();
                        const nextStatus = assignment.status === "Completed" ? "Pending" : "Completed";
                        const data = await api("/api/assignments/status", {
                            method: "POST",
                            body: JSON.stringify({ username, index: assignment.index, status: nextStatus })
                        });
                        applyDashboard(data);
                    } catch (error) {
                        alert(error.message);
                    }
                });
                container.appendChild(row);
            });
        }
    }

    // Overview recent-assignment table.
    const table = $(".assignment-table");
    if (!table) return;
    const oldRows = table.querySelectorAll(".table-row:not(.table-heading)");
    oldRows.forEach(row => row.remove());
    assignments.slice(0, 3).forEach((assignment) => {
        const row = document.createElement("div");
        row.className = "table-row";
        const statusClass = assignment.status === "Completed" ? "completed" : "pending";
        row.innerHTML = `<span></span><span>Academic</span><span class="${statusClass}">${assignment.status}</span>`;
        row.querySelector("span").textContent = assignment.name;
        table.appendChild(row);
    });
    if (!assignments.length) {
        const row = document.createElement("div");
        row.className = "table-row";
        row.innerHTML = `<span>No assignments yet.</span><span>—</span><span>—</span>`;
        table.appendChild(row);
    }
}

async function refreshDashboard() {
    const data = await getDashboard();
    applyDashboard(data);
    return data;
}

async function migrateLegacyData(data) {
    const rawUser = localStorage.getItem(LEGACY_USER_KEY);
    const rawAssignments = localStorage.getItem(LEGACY_ASSIGNMENTS_KEY);
    if (!rawUser) return data;

    let legacyUser = null;
    let legacyAssignments = [];
    try { legacyUser = JSON.parse(rawUser); } catch (_) {}
    try { legacyAssignments = rawAssignments ? JSON.parse(rawAssignments) : []; } catch (_) {}
    if (!legacyUser || legacyUser.username !== currentUsername()) return data;

    const flag = `smartAcademicMigrated_${legacyUser.username}`;
    if (!localStorage.getItem(flag)) {
        for (const assignment of legacyAssignments) {
            if (assignment && assignment.name) {
                try {
                    await api("/api/assignments/add", {
                        method: "POST",
                        body: JSON.stringify({ username: legacyUser.username, name: assignment.name })
                    });
                    if (assignment.status === "Completed") {
                        const fresh = await api(`/api/dashboard?username=${encodeURIComponent(legacyUser.username)}`);
                        const target = fresh.assignments.find(a => a.name === assignment.name && a.status === "Pending");
                        if (target) {
                            await api("/api/assignments/status", { method:"POST", body:JSON.stringify({username:legacyUser.username,index:target.index,status:"Completed"}) });
                        }
                    }
                } catch (_) {}
            }
        }
        if (legacyUser.skill) {
            try { await api("/api/career", { method:"POST", body:JSON.stringify({username:legacyUser.username,skill:legacyUser.skill}) }); } catch (_) {}
        }
        localStorage.setItem(flag, "1");
    }
    return refreshDashboard();
}

// LOGIN
loginForm?.addEventListener("submit", async (event) => {
    event.preventDefault();
    setMessage(loginMessage, "Signing in…");

    try {
        const data = await api("/api/login", {
            method: "POST",
            body: JSON.stringify({
                username: $("#username").value.trim(),
                password: $("#password").value
            })
        });
        setCurrentUsername(data.user.username);
        showDashboard();
        applyDashboard(data);
        const migrated = await migrateLegacyData(data);
        applyDashboard(migrated);
        setMessage(loginMessage, "");
    } catch (error) {
        setMessage(loginMessage, error.message || "Login failed.");
    }
});

// REGISTER
registerForm?.addEventListener("submit", async (event) => {
    event.preventDefault();
    setMessage(registerMessage, "Creating account…");

    const payload = {
        name: $("#registerName").value.trim(),
        age: Number($("#registerAge").value),
        department: $("#registerDepartment").value.trim(),
        username: $("#registerUsername").value.trim(),
        password: $("#registerPassword").value,
        gpa: Number($("#registerGpa").value),
        attendance: Number($("#registerAttendance").value)
    };

    try {
        const data = await api("/api/register", { method:"POST", body:JSON.stringify(payload) });
        setCurrentUsername(data.user.username);
        showDashboard();
        await refreshDashboard();
        registerForm.reset();
        setMessage(registerMessage, "");
    } catch (error) {
        setMessage(registerMessage, error.message || "Registration failed.");
    }
});

$("#showRegisterBtn")?.addEventListener("click", showRegister);
$("#showLoginBtn")?.addEventListener("click", showLogin);

// ACADEMIC UPDATE
$("#updateAcademicBtn")?.addEventListener("click", async () => {
    const username = currentUsername();
    try {
        const gpa = Number($("#updateGpa").value);
        const attendance = Number($("#updateAttendance").value);
        if (!Number.isFinite(gpa) || gpa < 0 || gpa > 4) throw new Error("GPA must be between 0.0 and 4.0.");
        if (!Number.isFinite(attendance) || attendance < 0 || attendance > 100) throw new Error("Attendance must be between 0 and 100.");

        const data = await api("/api/academic/update", { method:"POST", body:JSON.stringify({username,gpa,attendance}) });
        applyDashboard(data);
        setMessage($("#academicUpdateMessage"), "Academic information updated successfully.", true);
    } catch (error) {
        setMessage($("#academicUpdateMessage"), error.message);
    }
});

// ADD ASSIGNMENT
$("#addAssignmentBtn")?.addEventListener("click", async () => {
    const input = $("#assignmentInput");
    const name = input?.value.trim();
    if (!name) { alert("Please enter assignment name."); return; }
    try {
        const data = await api("/api/assignments/add", { method:"POST", body:JSON.stringify({username:currentUsername(),name}) });
        input.value = "";
        applyDashboard(data);
    } catch (error) { alert(error.message); }
});

// CAREER ASSESSMENT
const skillMap = {
    "C++": "C++",
    "HTML / CSS / JS": "HTML CSS JavaScript",
    "WordPress": "WordPress",
    "Python": "Python",
    "Database": "Database",
    "Networking": "Networking",
    "Cyber Security": "Cyber Security",
    "UI/UX Design": "UI/UX Design"
};

$$(".skill-option").forEach((button) => {
    button.addEventListener("click", async () => {
        const skill = skillMap[button.textContent.trim()];
        if (!skill) return;
        try {
            const data = await api("/api/career", { method:"POST", body:JSON.stringify({username:currentUsername(),skill}) });
            applyDashboard(data);
            alert("Career assessment updated!");
        } catch (error) { alert(error.message); }
    });
});

// GPA CALCULATOR — calculation is performed by C++.
$("#calculateGpaBtn")?.addEventListener("click", async () => {
    try {
        const percentage = Number($("#percentageInput").value);
        const data = await api("/api/gpa", { method:"POST", body:JSON.stringify({percentage}) });
        $("#gpaResult").textContent = `Calculated GPA: ${Number(data.gpa).toFixed(2)}`;
    } catch (error) { alert(error.message); }
});

// EXAM COUNTDOWN is a UI-only utility because no exam date is persisted in the current data model.
$("#countdownBtn")?.addEventListener("click", () => {
    const days = Number($("#daysInput").value);
    if (!Number.isFinite(days) || days < 0) { alert("Enter a valid number of days."); return; }
    $("#countdownResult").textContent = `${days} day(s) remaining`;
});

// NAVIGATION
$$('.nav-item').forEach((item) => {
    item.addEventListener("click", async () => {
        const target = item.dataset.section;
        if (!target) return;
        $$('.nav-item').forEach(x => x.classList.remove("active"));
        item.classList.add("active");
        $$('.dashboard-section').forEach(x => x.classList.remove("active-section"));
        const section = $("#" + target);
        if (section) section.classList.add("active-section");
        if (target === "overview" || target === "assignments" || target === "profile" || target === "career") {
            try { await refreshDashboard(); } catch (error) { console.error(error); }
        }
    });
});

$(".assignments-preview .text-btn")?.addEventListener("click", () => {
    $("[data-section='assignments']")?.click();
});

// LOGOUT
$("#logoutBtn")?.addEventListener("click", () => {
    setCurrentUsername(null);
    if (loginForm) loginForm.reset();
    showLogin();
});

// STARTUP
(async function init() {
    showLogin();
    const username = currentUsername();
    if (!username) return;
    try {
        showDashboard();
        const data = await refreshDashboard();
        await migrateLegacyData(data);
    } catch (_) {
        setCurrentUsername(null);
        showLogin();
    }
})();
