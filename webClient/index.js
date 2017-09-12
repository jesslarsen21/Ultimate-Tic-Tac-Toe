var Body = React.createClass({
	render: function() {
		return (
			<div>
				< Nav/>
				< MainContent/>
			</div>
		);
	}
});

var Nav = React.createClass({
	render: function() {
		return (
			<nav>
				<div className="nav-wrapper logoAdj blue">
					<a href="#" className="brand-logo">
						Ultimate-Tic-Tac-Toe
					</a>
				</div>
			</nav>
		);
	}
});

var MainContent = React.createClass({
	parseMessage: function(evt) {
		var data = evt.data.split(" ");
		console.log(data);
		if(data[0] == "loginreply") {
			if(data[1] == "valid") {
				this.setState({ login: true });
			}
		}
		if(data[0] == "signupreply") {
			if(data[1] == "valid" && data[2] == "valid" && data[3] == "match") {
				var req = "login " + this.state.user_email + " " + this.state.user_pw;
				this.state.ws.send(req);
			}
		}
	},
	setSignup: function(email, pw) {
		this.setState({user_email:email, user_pw:pw});
	},
	getInitialState: function() {
		var WS = new WebSocket("ws://rookie.cs.dixie.edu:3334")
		WS.onmessage = this.parseMessage;
		WS.onopen = function() {
			console.log("Connected");
		}
		WS.onerror = function (err) {
			console.log(err);
			console.log("on error called");
			setTimeout(function() {
				console.log("error timeout");
				//WS.close();
			}, 5000);
		}
		WS.onclose = function () {
			console.log("I tried to close");
			console.log(WS);
			setTimeout(function() {
				console.log("close timeout");
			}, 8000);
		}
		return (
			{login: false, ws: WS, user_email:"", user_pw:"", name:""}
		);
	},
	render: function() {
		var content;
		if(this.state.login) {
			content = < GameDisplay ws={this.state.ws} />;
		} else {
			content = < Auth ws={this.state.ws} signupcb={this.setSignup} />;
		}
		return (
			<div className="contentContainer row s6">
				{ content }
			</div>
		);
	}
});

var Auth = React.createClass({
	authHandler: function(req) {
		this.props.ws.send(req);
	},
	render: function() {
		return (
			<div className="container">
				< Login callback={this.authHandler} setParams={this.props.signupcb} />
				< SignUp callback={this.authHandler} setParams={this.props.signupcb} />
			</div>
		);
	}
});

var Login = React.createClass({
	getInitialState: function() {
		return (
			{email: "", password: ""}
		);
	},
	handleEmail: function(data) {
		this.setState({email: data.target.value});
	},
	handlePassword: function(data) {
		this.setState({password: data.target.value});
	},
	loginReq: function() {
		var req = "login " + this.state.email + " " + this.state.password;
		this.props.setParams(this.state.email, this.state.password);
		this.props.callback(req);
	},
	render: function() {
		return (
			<form className="row col offset-s1 s4 z-depth-2" id="login">
				<h3>Login</h3>
				<div className="input-field">
					<input placeholder="Email" type="email" name="lemail" onChange={this.handleEmail}/>
				</div>
				<div className="input-field">
					<input placeholder="Password" type="password" name="lpwd" onChange={this.handlePassword}/>
				</div>
				<a className="waves-effect waves-light blue btn" onClick={this.loginReq} >Submit</a>
			</form>
		);
	}
});

var SignUp = React.createClass({
	getInitialState: function() {
		return (
			{email: "", name: "", password: "", confirm: ""}
		);
	},
	handleEmail: function(data) {
		this.setState({email: data.target.value});
	},
	handleName: function(data) {
		this.setState({name: data.target.value});
	},
	handlePassword: function(data) {
		this.setState({password: data.target.value});
	},
	handleConfirm: function(data) {
		this.setState({confirm: data.target.value});
	},
	signupReq: function() {
		var req = "signup " + this.state.name + " " + this.state.email + " " + this.state.password + " " + this.state.confirm;
		this.props.setParams(this.state.email, this.state.password);
		this.props.callback(req);
	},
	render: function() {
		return (
			<form className="row col offset-s2 s4 z-depth-2" id="signup">
				<h3>Sign up</h3>
				<div className="input-field">
					<input placeholder="Email" type="email" name="semail" onChange={this.handleEmail}/>
				</div>
				<div className="input-field">
					<input placeholder="User Name" type="text" name="sname" onChange={this.handleName}/>
				</div>
				<div className="input-field">
					<input placeholder="Password" type="password" name="spwd" onChange={this.handlePassword}/>
				</div>
				<div className="input-field">
					<input placeholder="Confirm Password" type="password" name="spwd2" onChange={this.handleConfirm}/>
				</div>
				<a className="waves-effect waves-light blue btn" onClick={this.signupReq} >Submit</a>
			</form>
		);
	}
});

var GameDisplay = React.createClass({
	getInitialState: function() {
		return (
			{userPiece: "", opponentPiece: "", curTurn: "", opponentName: "Opponent", yourName: "Your Name"}
		);
	},
	setData: function(data) {
		this.setState(data);
	},
	render: function() {
		return (
			<div className="container">
				<div className="row">
					< Game ws={this.props.ws} dataCallback={this.setData} />
					< Display userPiece={this.state.userPiece} curTurn={this.state.curTurn} opponentPiece={this.state.opponentPiece} opponentName={this.state.opponentName} yourName={this.state.yourName}/>
				</div>
			</div>
		);
	}
});

var Display = React.createClass({
	render: function() {
		var uPiece = "grey";
		var oPiece = "grey";
		var turn = "grey";
		if(this.props.userPiece == "X") {
			uPiece = "red accent-2";
		} else if(this.props.userPiece == "O") {
			uPiece = "light-blue accent-2";
		}
		if(this.props.curTurn == "X") {
			turn = "red accent-2";
		} else if(this.props.curTurn == "O") {
			turn = "light-blue accent-2";
		}
		if(this.props.opponentPiece == "X") {
			oPiece = "red accent-2";
		} else if(this.props.opponentPiece == "O") {
			oPiece = "light-blue accent-2";
		}
		var uPieceClass = "row s2 z-depth-2 displayBoard " + uPiece;
		var oPieceClass = "row s2 z-depth-2 displayBoard " + oPiece;
		var turnClass = "row s2 z-depth-2 displayBoard " + turn;
		return (
			<div className="col offset-s1">
				<div className={uPieceClass}>
					<h5>{ this.props.yourName }</h5>
					<div className="displayItem">
						<h3 id="userPiece">{ this.props.userPiece }</h3>
					</div>
				</div>
				<div className={oPieceClass}>
					<h5> { this.props.opponentName } </h5>
					<div className="displayItem">
						<h3 id="opponentPiece">{ this.props.opponentPiece }</h3>
					</div>
				</div>
				<div className={turnClass}>
					<h5>Current Turn</h5>
					<div className="displayItem">
						<h3 id="curTurn">{ this.props.curTurn }</h3>
					</div>
				</div>
			</div>
		);
	}
});

var Game = React.createClass({
	componentDidMount: function() {
		StartGame(this.props.ws, this.props.dataCallback);
	},
	render: function() {
		return (
			<div id="gameContainer" className="col" >
				<canvas className="z-depth-2" id="UTTT" width="500" height="500"></canvas>
			</div>
		);
	}
});

React.render(
	<Body />,
	document.body
);
